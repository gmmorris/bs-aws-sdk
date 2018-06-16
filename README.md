# bs-aws-sdk ![alt TravisCI Build](https://travis-ci.org/gmmorris/bs-aws-sdk.svg?branch=master)

Bindings for the JavaScript AWS SDK for [BuckleScript](https://github.com/bloomberg/bucklescript) in [Reason](https://github.com/aws/aws-sdk-js).


## Status
I have only implemented bindings for the AWS S3 upload function, as writing bindings for the AWS SDK is a full time job and so, for now, I'm just implementing what I need..
*PRs* are more than welcome for additional bindings.
<br />

## Installation

```sh
npm install --save-dev @gmmorris/bs-aws-sdk
```

Then add `@gmmorris/bs-aws-sdk` to `bs-dependencies` in your `bsconfig.json`:
```js
{
  ...
  "bs-dependencies": ["@gmmorris/bs-aws-sdk"]
}
```
<br />