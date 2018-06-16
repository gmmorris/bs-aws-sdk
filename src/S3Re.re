[@bs.deriving jsConverter]
type acl = [
  | [@bs.as "private"] `Private
  | [@bs.as "public-read"] `PublicRead
  | [@bs.as "public-read-write"] `PublicReadWrite
  | [@bs.as "authenticated-read"] `AuthenticatedRead
  | [@bs.as "aws-exec-read"] `AwsExecRead
  | [@bs.as "bucket-owner-read"] `BucketOwnerRead
  | [@bs.as "bucket-owner-full-control"] `BucketOwnerFullControl
];

[@bs.deriving jsConverter]
type parsedUploadParams = {
  /*  The canned ACL to apply to the object. */
  _ACL: acl,
  /* Object data. */
  _Body: string,
  /* Name of the bucket to which the PUT operation was initiated. */
  _Bucket: string,
  /* A standard MIME type describing the format of the object data. */
  _ContentType: string,
  /* Object key for which the PUT operation was initiated. */
  _Key: string,
};

[@bs.obj]
external uploadParams :
  (
    ~_ACL: string=?,
    ~_Body: string=?,
    ~_Bucket: string=?,
    ~_ContentType: string=?,
    ~_Key: string=?,
    unit
  ) =>
  parsedUploadParams =
  "";

let uploadParams =
    (~accessControlPolicy=?, ~body=?, ~bucket=?, ~contentType=?, ~key=?, ()) =>
  uploadParams(
    ~_ACL=?Belt.Option.map(accessControlPolicy, aclToJs),
    ~_Body=?body,
    ~_Bucket=?bucket,
    ~_ContentType=?contentType,
    ~_Key=?key,
    (),
  );

module ManagedUpload = {
  module SendData = {
    type t;
    /* URL of the uploaded object. */
    [@bs.get] external location : t => string = "Location";
    /* ETag of the uploaded object. */
    [@bs.get] external eTag : t => string = "ETag";
    /* Bucket to which the object was uploaded. */
    [@bs.get] external bucket : t => string = "Bucket";
    /* Key to which the object was uploaded. */
    [@bs.get] external key : t => string = "Key";
  };
};

type t;

[@bs.send.pipe: t] external upload : parsedUploadParams => unit = "";

[@bs.send.pipe: t]
external uploadWithCallback :
  (
    parsedUploadParams,
    [@bs.uncurry] (
      (Js.Nullable.t(Js.Exn.t), ManagedUpload.SendData.t) => unit
    )
  ) =>
  unit =
  "upload";

[@bs.module ("aws-sdk", "AwsSdk")] [@bs.new] external s3 : unit => t = "S3";